<?php

namespace App\Http\Controllers\Auth;

use App\User;
use Validator;
use App\Http\Controllers\Controller;
use Illuminate\Foundation\Auth\RegistersUsers;

class RegisterController extends Controller
{
    /*
    |--------------------------------------------------------------------------
    | Register Controller
    |--------------------------------------------------------------------------
    |
    | This controller handles the registration of new users as well as their
    | validation and creation. By default this controller uses a trait to
    | provide this functionality without requiring any additional code.
    |
    */

    use RegistersUsers;

    /**
     * Where to redirect users after login / registration.
     *
     * @var string
     */
    protected $redirectTo = '/home';

    /**
     * Create a new controller instance.
     *
     * @return void
     */
    public function __construct()
    {
        $this->middleware('guest');
    }

    /**
     * Get a validator for an incoming registration request.
     *
     * @param  array  $data
     * @return \Illuminate\Contracts\Validation\Validator
     */
    protected function validator(array $data)
    {
        return Validator::make($data, [
            'name' => 'required|max:255|unique:users',
            'password' => 'required|min:6|confirmed',
        ]);
    }

    /**
     * Create a new user instance after a valid registration.
     *
     * @param  array  $data
     * @return User
     */
    protected function create(array $data)
    {
        $user = new User;
        $user->name = $data['name'];
        $user->save();

        $this->computeAndSavePassword($user, $data['password']);
        $this->setUserRole($user);

        return $user;
    }

    /**
     * Compute the hashed password of the user
     *
     * @param User $user
     * @param $password
     */
    private function computeAndSavePassword(User $user, $password)
    {
        $user->password = bcrypt($password);
        // Make the race condition easy enough to exploit and save some CPU
        // compared to a long bcrypt operation
        sleep(2);
        $user->save();
    }


    /**
     * Set the user role
     * For now, this is easy, the user can only be a simple user
     *
     * @param User $user
     */
    private function setUserRole(User $user)
    {
        $user->role = User::REGISTERED;
        $user->save();
    }
}
